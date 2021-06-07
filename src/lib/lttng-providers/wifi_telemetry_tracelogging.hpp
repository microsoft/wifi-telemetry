/**
 * This file should be used in place of #include'ing TraceLoggingProvider.h
 * directly because it re-defines as macros what LTTNG has defined as an enum
 * :-(
 */
#include <tracelogging/TraceLoggingProvider.h>

#undef TRACE_EMERG
#undef TRACE_ALERT
#undef TRACE_CRIT
#undef TRACE_ERR
#undef TRACE_WARNING
#undef TRACE_NOTICE
#undef TRACE_INFO
#undef TRACE_DEBUG_SYSTEM
#undef TRACE_DEBUG_PROGRAM
#undef TRACE_DEBUG_PROCESS
#undef TRACE_DEBUG_MODULE
#undef TRACE_DEBUG_UNIT
#undef TRACE_DEBUG_FUNCTION
#undef TRACE_DEBUG_LINE
#undef TRACE_DEBUG
