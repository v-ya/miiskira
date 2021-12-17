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
	"miiskira$posky$stamp" @null
	"miiskira$type$posky_task" @null
	"miiskira$type$posky_return" @null
	"miiskira$type$posky_clear_lonely" @null
	"miiskira_posky_gen_type" @null
	"miiskira_posky_feeding" @null
	"miiskira_posky_feeding_proxy_adorable" @null
	"miiskira_candy_alloc__posky_task" @null
	"miiskira_candy_alloc__posky_return" @null
	"miiskira_candy_append__posky_task" @null
	"miiskira_candy_append__posky_return" @null
	"miiskira_candy_alloc__posky_clear_lonely" @null
