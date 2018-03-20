#ifndef T_SYSLOG_H
#define T_SYSLOG_H

/* syslog level */
#define LOG_MASK(X)	(1U<<(X))
#define LOG_UPTO(X)	((1U<<((X)+1))-1)

#define LOG_ENTER	0
#define LOG_EXIT	0x80

#define LOG_EMERG	0
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERROR	3
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7

#endif	/* T_SYSLOG_H */
