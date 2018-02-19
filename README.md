# gdnative_dodge_minimal

Will one day be a minimal GDNative C implementation of "Dodge the Creeps" 
tutorial game from Godot docs.  Right now it doesn't work (likely due to
my questionable C skills).

Issues:

1. How do you access variables & properties on Godot objects? godot_api.json shows getter/setter methods - is that the way?
1. Why would I Object.callv("emit_signal") (like in https://github.com/GodotNativeTools/GDNative-demos/blob/master/c/GLFWDemo/src/init.c#L51) instead of Object.emit_signal?
1. Get basic Node.get_node and Node.get_path to work
