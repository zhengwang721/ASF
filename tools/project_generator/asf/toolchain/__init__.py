from asf.helper import using_ironpython

all_modules = ['doxygen', 'doxygen_v2', 'avrgcc', 'armgcc', 'generic', 'iarew', 'iarewarm', 'verbose', 'avrstudio5']

# Workaround is needed for IronPython imports
if using_ironpython():
	for module in all_modules:
		__import__('%s.%s' % (__name__, module))
else:
	__all__ = all_modules
