: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya]
	"name" @string => "miiskira.posky"
[kiya "like"]
	"import.refer" @null
	"import.queue" @null
	"import.posky" @null
	"import.vattr" @null
	"import.yaw" @null
[kiya "dylink"]
	"posky" @"dylink" => &"posky.dy"
[kiya "parse"]
	"kiya.export" @null
[kiya]
	"initial" @string => "miiskira_posky_initial"
	"finally" @string => "miiskira_posky_finally"

["kiya.export"]
	"miiskira_posky_gen_type" @null
