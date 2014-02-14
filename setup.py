try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

readme_file = "README.md"
license_file = "LICENSE"
authors_file = "AUTHORS"

with open(readme_file) as f:
    readme = f.read()

with open(license_file) as f:
    license = f.read()

with open(authors_file) as f:
    authors = f.read()

setup(
    name='Umons Examination Timetable',
    description='Tool for generating examination timetable',
    long_description=readme,
    author=authors,
    url='',
    version='0.1',
    packages=['examtimetable'],
    license=license,
)
