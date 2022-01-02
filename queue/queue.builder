: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya]
	"name" @string => "miiskira.queue"
[kiya "like"]
	"miiskira.log" @null
	"import.refer" @null
	"import.queue" @null
	"import.yaw" @null
[kiya "dylink"]
	"queue" @"dylink" => &"queue.dy"
[kiya "parse"]
	"kiya.export" @null

["kiya.export"]
	"miiskira_queue_alloc" @null
