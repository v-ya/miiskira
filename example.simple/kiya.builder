: verify   = "*>^~64.4"
: tag      = "kiya"
: version  = "miiskira"
: author   = "vya"

[$ "kiya" kiya]
	"name" @string => "miiskira.example.simple"
[kiya "like"]
	"miiskira.posky" @null
	"miiskira.graph" @null
	"xwin.pool" @null
	"console.main" @null
[kiya "dylink"]
	"main" @"dylink" => &"main.dy"
[kiya "parse"]
	"xwin.pool.parse" @null
[kiya]
	"initial" @string => "initial"
	"finally" @string => "finally"

["xwin.pool.parse" "miiskira.example.simple"]
	"title"         @string => "miiskira 简单例子"
	"data.allocer"  @string => "get_inst"
	"event.close"   @string => "event_close"
	"event.expose"  @string => "event_expose"
	"event.key"     @string => "event_key"
	"event.button"  @string => "event_button"
	"event.pointer" @string => "event_pointer"
	"event.config"  @string => "event_config"
