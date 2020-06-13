import sys
from glob import glob
from os import path
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

with open(path.join(path.abspath(path.dirname(__file__)), "README.md")) as f:
    readme = f.read()


class GetPybindInclude(object):
    def __str__(self):
        import pybind11
        return pybind11.get_include()


class BuildExt(build_ext):
    def build_extension(self, ext):
        if self.compiler.compiler_type == "unix":
            opts = ['-g3', '-O3', '-fvisibility=hidden', '-std=c++17']
        else:
            opts = ['/O2', '/Oi', '/Ot', '/arch', '/Zi']

        if sys.platform == 'darwin':
            opts.append('-stdlib=libc++')

        ext.define_macros = [("NDEBUG", '1')]
        ext.extra_compile_args = opts

        build_ext.build_extension(self, ext)


setup(
    name="pvzemu",
    version="0.9.0",
    author="testla",
    url="https://github.com/dnartz/PvZ-Emulator",
    description="Plants vs. Zombies Survival Endless emulator.",
    long_description=readme,
    long_description_content_type="text/markdown",
    ext_modules=[Extension(
        "pvzemu",
        sorted(
            glob("object/*.cpp") +
            glob("learning/*.cpp") +
            glob("system/*.cpp") +
            glob("system/**/*.cpp") + [
                "pybind.cpp",
                "world.cpp"
            ]),
        include_dirs=[GetPybindInclude(), "./"],
        language="c++"
    )],
    setup_requires=['pybind11>=2.5.0'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False
)
