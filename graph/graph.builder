: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya]
	"name" @string => "miiskira.graph"
[kiya "like"]
	"miiskira.posky" @null
	"import.mlog" @null
	"import.hashmap" @null
	"import.exbuffer" @null
	"import.pocket" @null
	"import.vattr" @null
	"import.graph" @null
	"import.fsys" @null
[kiya "dylink"]
	"graph" @"dylink" => &"graph.dy"
[kiya "parse"]
	"kiya.export" @null
[kiya]
	"initial" @string => "miiskira_graph_initial"
	"finally" @string => "miiskira_graph_finally"

["kiya.export"]
	# type
	"miiskira$type$graph_present_create" @null
	"miiskira$type$graph_present_destroy" @null
	"miiskira$type$graph_present_resize" @null
	"miiskira$type$graph_present_do" @null
	# function
	"miiskira_graph_present_create_bgra" @null
	"miiskira_graph_present_destroy" @null
	"miiskira_graph_present_resize" @null
	"miiskira_graph_present_do" @null
