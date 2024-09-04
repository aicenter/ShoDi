import setuptools
from setuptools import setup

setup(
	name='shortestdistances',
	version='0.0.0',
	description='This project implements multiple methods for shortest distance '
    'computation in directed weighted graphs that leverage preprocessing of the '
    'graph to provide fast shortest distance queries.',
	author='Michal Cvach',
	author_email='',
	license='GNU GPLv3',
	packages=setuptools.find_packages(),
	install_requires=[
		'numpy',
		'pandas',
		'plotly',
		'kaleido',
        'geojson',
        'argparse'
	],
	python_requires='>=3.8'
)
