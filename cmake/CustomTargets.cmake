##---------------------------------------------------------------------------
## Author:      Daniel Williams
## License:     Public Domain
##---------------------------------------------------------------------------
##
##   Custom targets to make certain development tasks easier
##

ADD_CUSTOM_TARGET( tail tail -f ~/.opencpn/opencpn.log
                  COMMENT 'tailing the OpenCPN log file....'
                  USES_TERMINAL)
