: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya]
	"name" @string => "miiskira.log"
[kiya "like"]
	"import.refer" @null
	"import.mlog" @null
	"import.exbuffer" @null
	"import.yaw" @null
[kiya "dylink"]
	"log" @"dylink" => &"log.dy"
[kiya "parse"]
	"kiya.export" @null
[kiya]
	"initial" @string => "miiskira_log_initial"
	"finally" @string => "miiskira_log_finally"

["kiya.export"]
	"miiskira$log$error" @null
	"miiskira$log$warning" @null
	"miiskira$log$info" @null
	"miiskira$log$verbose" @null
	"miiskira_log_clear" @null
	"miiskira_log_dump" @null
