import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="megabas",
    version="1.0.1",
    author="Sequent Microsystems",
    author_email="olcitu@gmail.com",
    description="A set of functions to control Sequent Microsystems Building Automation Card for Raspberry Pi",
	license='MIT',
    url="https://sequentmicrosystems.com/collections/all-io-cards/products/raspberry-pi-building-automation",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 2/3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)