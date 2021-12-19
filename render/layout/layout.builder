: verify   = "*>^~64.4"
: tag      = "miiskira.render"
: version  = "miiskira"
: author   = "vya"

["layout" layout]
# vertex
[layout "vertex-p3c3"]
	"position" @"vec3" => (f32[3]) {}
	"color" @"vec3" => (f32[3]) {}
# input output
[layout "inner-c4"]
	"color" @"vec4" => (f32[4]) {}
# uniform
[layout "viewport"]
	"viewport" @"mat4" => (f32[16]) {}
