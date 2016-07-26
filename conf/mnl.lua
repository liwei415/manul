--mnl server config

--是否守护进程运行
is_daemon = 0

--server config
--绑定IP
ip = '0.0.0.0'
--端口
port = 9528
--运行线程数(CPU数)
thread_num = 2
backlog_num = 1024
max_keepalives  = 1
--retry = 3
system = io.popen('uname -sn'):read('*l')
pwd = io.popen('pwd'):read('*l')

--header config
--返回时所带的HTTP header
headers = 'Cache-Control:max-age=7776000'

--access config
--support mask rules like 'allow 10.1.121.138/24'
--NOTE: remove rule can improve performance
--接口的IP控制权限，将权限规则注释掉可以提升服务器处理能力，下同
access_rule   = 'allow 0.0.0.0; 192.168.11.0/24; 192.168.1.111'

--log config
--log_level output specified level of log to logfile
--[[
LOG_FATAL 0     System is unusable
LOG_ALERT 1     Action must be taken immediately
LOG_CRIT 2      Critical conditions
LOG_ERROR 3     Error conditions
LOG_WARNING 4   Warning conditions
LOG_NOTICE 5    Normal, but significant
LOG_INFO 6      Information
LOG_DEBUG 7     DEBUG message
]]
--输出log级别
log_level = 7
--输出log路径
log_path = pwd .. '/mnl.log'

--请求数据大小限制，默认10MB
max_size = 10*1024*1024

--remote server config
--绑定IP
remote_ip = ''
--端口
remote_port = 9993

--平台
plat_code = ''
plat_key = ''

--ftp
ftp_user = ''
ftp_pass = ''
ftp_port = 9955
