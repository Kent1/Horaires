try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

readme_file = "README.md"
license_file = "LICENSE"
authors_file = "AUTHORS"

with open(readme_file) as f:
    readme = f.read()

with open(license_file) as f:
    license = f.read()

with open(authors_file) as f:
    authors = f.read()

src = [
    'lib/graph_heuristics.c',
    'lib/ils_heuristics.c',
    'lib/room_assign.c',
    'lib/structs.c',
    'lib/preprocessing.c'
]

lib = Extension(
    'lib.libtimetable',
    sources=src,
    extra_compile_args=['-Wall', '-O', '-std=c99', '-fPIC', '-fshort-enums']
)

setup(
    name='Umons Examination Timetable',
    description='Tool for generating examination timetable',
    long_description=readme,
    author=authors,
    url='',
    version='0.1',
    packages=['examtimetable', 'examtimetable.c'],
    ext_modules=[lib],
    license=license,
)
