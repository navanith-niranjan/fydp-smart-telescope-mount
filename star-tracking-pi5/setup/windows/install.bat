@echo off
:: Activate virtual environment
call venv\Scripts\activate

:: Install Python packages
pip install --upgrade pip
pip install opencv-contrib-python
pip install psutil
pip install imageio
pip install astropy
pip install pandas
pip install statistics
pip install astroquery
pip install matplotlib
pip install setuptools --upgrade
pip install scipy
pip install pyueye

:: Install the local star_tracker module
cd star-tracking-pi5\py_src\star_tracker
pip install .
cd ..\..