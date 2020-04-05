import os
import shutil
import subprocess
from pathlib import Path

from setuptools import setup, Command, glob
from typing import Optional, Tuple, List


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


def get_built_paths(build_dir: Path) -> Optional[List[Path]]:
    lib_dir = build_dir / 'lib' / 'volume_augmentations'
    bin_dir = build_dir / 'bin'
    if not lib_dir.exists():
        return None

    va_cpp_path = None
    va_rs_path = None
    def check_extension(filename: str):
        if filename.endswith(".so") or filename.endswith('.pyd') or filename.endswith('.dll'):
            return True
        return False

    dirs = [lib_dir, bin_dir]
    for dir in dirs:
        for entry in dir.iterdir():
            if entry.name.startswith('va_cpp') and check_extension(entry.name):
                va_cpp_path = entry
            elif entry.name.startswith('va_rs') and check_extension(entry.name):
                va_rs_path = entry

    return [va_cpp_path, va_rs_path] if all((va_cpp_path, va_rs_path)) else None


def compile_extensions() -> List[str]:
    source_dir, build_dir = get_source_and_build_dir()
    built_paths = get_built_paths(build_dir)
    if built_paths is not None:
        return list(map(str, built_paths))

    try:
        subprocess.check_output(['cmake', '--version'])
    except OSError:
        raise RuntimeError('CMake must be installed to build this project')

    build_dir.mkdir(exist_ok=True)
    #  print(os.environ)
    try:
        subprocess.check_call(['cmake', '-G', 'Ninja', str(source_dir)], cwd=str(build_dir), env=os.environ)
        subprocess.check_call(['cmake', '--build', '.'], cwd=str(build_dir), env=os.environ)
    except subprocess.CalledProcessError:
        raise RuntimeError('Running CMake failed')

    built_paths = get_built_paths(build_dir)
    if built_paths is None:
        raise RuntimeError('Compiled libraries not found')


compile_extensions()
setup(
    name='volume-augmentations',
    version='0.1.0',
    packages=['volume_augmentations'],
    package_dir={'volume_augmentations': 'va_py/volume_augmentations'},
    cmdclass={
        'clean': CleanCommand,
    },
    zip_safe=False,
    install_requires=['numpy~=1.18.2'],
)
