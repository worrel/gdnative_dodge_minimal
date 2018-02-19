#include <gdnative_api_struct.gen.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

// instance constructor/destructor
GDCALLINGCONV void *player_constructor(godot_object *p_instance, void *p_method_data);
GDCALLINGCONV void player_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);

// callback methods
godot_variant ready(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant process(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);

// call-out methods
void set_animation(godot_object *animated_sprite, char *anim_name);
godot_node_path get_node_path(godot_object *instance);
godot_object *get_node(godot_object *instance, wchar_t *node_name);
godot_string get_node_name(godot_object *instance);

// library init
void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	api = p_options->api_struct;

	// now find our extensions
	for (int i = 0; i < api->num_extensions; i++) {
		switch (api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];
			}; break;
			default: break;
		};
	};	
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	api = NULL;
	nativescript_api = NULL;
}

// register constructor & callback methods
void GDN_EXPORT godot_nativescript_init(void *p_handle) {
	// register Player class
	godot_instance_create_func create = { NULL, NULL, NULL };
	create.create_func = &player_constructor;

	godot_instance_destroy_func destroy = { NULL, NULL, NULL };
	destroy.destroy_func = &player_destructor;

	nativescript_api->godot_nativescript_register_class(p_handle, "Player", "Area2D", create, destroy);

	// standard method attributes, TODO: OK to re-use?
	godot_method_attributes attributes = { GODOT_METHOD_RPC_MODE_DISABLED };

	// register Player._ready()
	godot_instance_method ready_method = { NULL, NULL, NULL };
	ready_method.method = &ready;
	nativescript_api->godot_nativescript_register_method(p_handle, "Player", "_ready", attributes, ready_method);

	// register Player._process()
	godot_instance_method process_method = { NULL, NULL, NULL };
	process_method.method = &process;
	nativescript_api->godot_nativescript_register_method(p_handle, "Player", "_process", attributes, process_method);
}

GDCALLINGCONV void *player_constructor(godot_object *p_instance, void *p_method_data) {
	printf("Player initialized\n");
	return NULL;
}

GDCALLINGCONV void player_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data) {
	printf("Player freed\n");
	// commenting out, no user data right now
	//api->godot_free(p_user_data);
}

godot_variant ready(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;

	printf( "ready!\n" );

	godot_string node_name = get_node_name(p_instance);

	// NOTE: prints an empty string?
	wprintf(L"Node name=%S\n", api->godot_string_wide_str(&node_name));

	// NOTE: commenting out because get_node_path fails to find Node::get_path method
	// godot_node_path player_path = get_node_path(p_instance);
	// godot_string player_path_gstring = api->godot_node_path_as_string(&player_path);
	// wprintf(L" Player path= %s\n", api->godot_string_wide_str(&player_path_gstring));

	// NOTE: call succeeds but doesn't find AnimatedSprite child node
	godot_object* as = get_node(p_instance, L"AnimatedSprite");

	// NOTE: commenting out because sprite node not found
	//set_animation(as, "up");

	return ret;
}

godot_variant process(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	printf( "process\n" );
	return ret;
}

// TODO: how to access instance variables? This returns empty string
godot_string get_node_name(godot_object *instance) {
	char *class_name = "Node";
	char *method_name = "get_name";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		printf("get method %s::%s\n", class_name, method_name);
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	printf("call method %s::%s\n", class_name, method_name);

	godot_variant v;
	const void *c_args[] = {};

	api->godot_method_bind_ptrcall(mb, instance, c_args, &v);

	godot_string nm = api->godot_variant_as_string(&v);
	api->godot_variant_destroy(&v);

	return nm;
}

godot_node_path get_node_path(godot_object *instance) {
	char *class_name = "Node";
	char *method_name = "get_path";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		printf("get method %s::%s\n", class_name, method_name);
		// NOTE: this call succeeds
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	printf("call method %s::%s\n", class_name, method_name);

	godot_variant v;
	const void *c_args[] = {};

	api->godot_method_bind_ptrcall(mb, instance, c_args, &v);

	godot_node_path np = api->godot_variant_as_node_path(&v);
	api->godot_variant_destroy(&v);

	return np;
}

godot_object *get_node(godot_object *instance, wchar_t *node_name) {
	char *class_name = "Node";
	char *method_name = "get_node";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		printf("get method %s::%s\n", class_name, method_name);
		// NOTE: this call succeeds
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	printf("call method %s::%s\n", class_name, method_name);

	godot_variant v;
	godot_node_path np;
	{
		godot_string name;
		api->godot_string_new_with_wide_string(&name, node_name, wcslen(node_name));
		api->godot_node_path_new(&np, &name);
		api->godot_string_destroy(&name);
	}
	const void *c_args[] = {
		&np
	};

	api->godot_method_bind_ptrcall(mb, instance, c_args, &v);
	api->godot_node_path_destroy(&np);

	godot_object *obj = api->godot_variant_as_object(&v);
	api->godot_variant_destroy(&v);

	return obj;
}

void set_animation(godot_object *animated_sprite, char *anim_name) {
	char *class_name = "AnimatedSprite";
	char *method_name = "set_animation";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		printf("get method %s::%s\n", class_name, method_name);
		// NOTE: this call succeeds
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	printf("call method %s::%s\n", class_name, method_name);

	godot_variant v;
	godot_string name = api->godot_string_chars_to_utf8(anim_name);
	const void *c_args[] = {
		&name
	};

	api->godot_method_bind_ptrcall(mb, animated_sprite, c_args, &v);
	api->godot_string_destroy(&name);
}