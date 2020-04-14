import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Optional, Tuple, List

from setuptools import setup, Command, glob


class CleanCommand(Command):
    CLEAN_FILES = ['build', 'dist', '*.pyc', '*.tgz', '*.egg-info']

    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        for path_spec in self.CLEAN_FILES:
            paths = glob.glob(os.path.normpath(path_spec))
            for path in [str(p) for p in paths]:
                print('removing {}'.format(os.path.relpath(path)))
                shutil.rmtree(path)


def get_source_and_build_dir() -> Tuple[Path, Path]:
    source_dir = Path(__file__).parent.absolute()
    return source_dir, source_dir / 'build'


def get_lib_dir(build_dir: Path) -> Path:
    return build_dir / 'lib' / 'volume_augmentations'


def get_bin_dir(build_dir: Path) -> Path:
    return build_dir / 'bin'


def get_built_paths(build_dir: Path) -> Optional[List[Path]]:
    lib_dir = get_lib_dir(build_dir)
    bin_dir = get_bin_dir(build_dir)
    if not lib_dir.exists():
        return None

    va_cpp_path = None
    # va_rs_path = None
    other_paths = []

    def check_extension(filename: str):
        return filename.endswith(".so") or filename.endswith('.pyd') or filename.endswith('.dll')

    dirs = [lib_dir, bin_dir]
    for directory in dirs:
        for entry in directory.iterdir():
            extension_is_valid = check_extension(entry.name)
            if entry.name.startswith('va_cpp') and extension_is_valid:
                va_cpp_path = entry
            # elif entry.name.startswith('va_rs') and extension_is_valid:
            #     va_rs_path = entry
            elif extension_is_valid:
                other_paths.append(entry)

    return [va_cpp_path, *other_paths] if va_cpp_path else None


def copy_libraries_to_lib_if_needed(built_paths: List[Path], build_dir: Path) -> List[Path]:
    lib_dir = get_lib_dir(build_dir)
    corrected_built_paths = []
    for built_path in built_paths:
        if lib_dir not in built_path.parents:
            new_path = lib_dir / built_path.name
            shutil.copy2(built_path, new_path)
            built_path = new_path

        corrected_built_paths.append(built_path)

    return corrected_built_paths


def compile_extensions() -> List[str]:
    source_dir, build_dir = get_source_and_build_dir()
    built_paths = get_built_paths(build_dir)
    if built_paths is not None:
        built_paths = copy_libraries_to_lib_if_needed(built_paths, build_dir)
        return list(map(str, built_paths))

    try:
        subprocess.check_output(['cmake', '--version'])
    except OSError:
        raise RuntimeError('CMake must be installed to build this project')

    build_dir.mkdir(exist_ok=True)
    try:
        python_version = f'{sys.version_info[0]}.{sys.version_info[1]}'
        subprocess.check_call(['cmake', '-G', 'Ninja', '-DPYTHON_VERSION=', python_version,
                               '-DPYBIND11_PYTHON_VERSION=', python_version, str(source_dir)],
                              cwd=str(build_dir), env=os.environ)
        subprocess.check_call(['cmake', '--build', '.'], cwd=str(build_dir), env=os.environ)
    except subprocess.CalledProcessError:
        raise RuntimeError('Running CMake failed')

    built_paths = get_built_paths(build_dir)
    if built_paths is None:
        raise RuntimeError('Compiled libraries not found')

    built_paths = copy_libraries_to_lib_if_needed(built_paths, build_dir)
    return list(map(str, built_paths))


try:
    from wheel.bdist_wheel import bdist_wheel as _bdist_wheel

    class bdist_wheel(_bdist_wheel):
        def finalize_options(self):
            super().finalize_options()
            self.root_is_pure = False
except ImportError:
    bdist_wheel = None


compile_extensions()
setup(
    name='volume-augmentations',
    version='0.1.0',
    packages=['volume_augmentations'],
    package_dir={'volume_augmentations': 'va_py/volume_augmentations'},
    cmdclass={
        'clean': CleanCommand,
        'bdist_wheel': bdist_wheel
    },
    zip_safe=False,
    install_requires=['numpy~=1.18.2'],
    extras_require={
        'tests': ['pytest', 'pytest-benchmark']
    },
)
