import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="megabas",
    version="1.0.0",
    author="Sequent Microsystems",
    author_email="olcit@gmail.com",
    description="A set of functions to control Sequent Microsystems Building Automation Card for Raspberry Pi",
	license='MIT',
    url="https://sequentmicrosystems.com/index.php?route=product/product&path=20&product_id=65",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 2/3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)