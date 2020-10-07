from distutils.core import setup
setup (name = 'pyds_analytics_meta',
       version = '1.0',
       description = """Install precompiled DeepStream Python bindings for nvds-analytics metadata extension""",
       packages=[''],
       package_data={'': ['pyds_analytics_meta.so']},
       )
