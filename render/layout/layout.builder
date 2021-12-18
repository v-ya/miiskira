: verify   = "*>^~64.4"
: tag      = "miiskira.render"
: version  = "miiskira"
: author   = "vya"

["layout" layout]
# vertex
[layout "vertex-p3c3"]
	"position" @"vec3" => (f32[3]) {}
	"color" @"vec3" => (f32[3]) {}
# uniform
[layout "viewport"]
	"viewport" @"mat4" => (f32[16]) { 1, 0, 0, 0,   0, 1, 0, 0,   0, 0, 1, 0,   0, 0, 0, 1 }
