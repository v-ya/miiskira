: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya]
	"name" @string => "miiskira.graph"
[kiya "like"]
	"miiskira.posky" @null
	"import.mlog" @null
	"import.graph" @null
[kiya "dylink"]
	"graph" @"dylink" => &"graph.dy"
# [kiya "parse"]
# 	"kiya.export" @null
[kiya]
	"initial" @string => "miiskira_graph_initial"
	"finally" @string => "miiskira_graph_finally"

# ["kiya.export"]
