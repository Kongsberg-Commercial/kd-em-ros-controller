from setuptools import find_packages
from setuptools import setup

setup(
    name='ros_kctrl_custom_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('ros_kctrl_custom_interfaces', 'ros_kctrl_custom_interfaces.*')),
)
