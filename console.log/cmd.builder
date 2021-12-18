: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya] "name" @string => "miiskira.console.log"

[kiya "like"]
	"miiskira.log" @null
	"console.main" @null

[kiya "dylink"]
	"cmd" @"dylink" => &"cmd.dy"

[kiya "parse"]
	"console.main.parse.command" @null

["console.main.parse.command"]
	"log"       @string => "cmd_alloc_log"
	"log.clear" @string => "cmd_alloc_log_clear"
