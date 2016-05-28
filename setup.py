from distutils.core import setup

setup(name="ecpy",
      version="1.0.1",
      description="A Elliptic-Curve Library",
      author="@elliptic_shiho",
      author_email="shiho.elliptic@gmaill.com",
      url="https://github.com/elliptic-shiho/ecpy/",
      packages=["ecpy", "ecpy.abstract", "ecpy.structure", "ecpy.algorithm"]
      )
