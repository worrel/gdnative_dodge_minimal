#include "dodge.h"
#include <gdnative_api_struct.gen.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// instance constructor/destructor
GDCALLINGCONV void *player_constructor(godot_object *p_instance, void *p_method_data);
GDCALLINGCONV void player_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);

// callback methods
godot_variant ready(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant process(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);

// call-out methods
void play(godot_object *animated_sprite);
void set_animation(godot_object *animated_sprite, char *anim_name);
godot_object *get_node(godot_object *instance, char *node_name);

void register_player(void *p_handle ) {
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
}

godot_variant ready(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;

	printf( "ready!\n" );

	godot_object* as = get_node(p_instance, "AnimatedSprite");
	set_animation(as, "up");

	return ret;
}

godot_variant process(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	printf( "process\n" );
	
	godot_object* as = get_node(p_instance, "AnimatedSprite");
	play(as);

	return ret;
}

godot_object *get_node(godot_object *instance, char *node_name) {
	char *class_name = "Node";
	char *method_name = "get_node";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	godot_object *obj;
	godot_node_path np;
	{
		godot_string name;
		api->godot_string_new(&name);
		api->godot_string_parse_utf8(&name, node_name);
		api->godot_node_path_new(&np, &name);
		api->godot_string_destroy(&name);
	}
	const void *c_args[] = {
		&np
	};

	api->godot_method_bind_ptrcall(mb, instance, c_args, &obj);
	api->godot_node_path_destroy(&np);

	return obj;
}

void set_animation(godot_object *animated_sprite, char *anim_name) {
	char *class_name = "AnimatedSprite";
	char *method_name = "set_animation";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	godot_string name;
	api->godot_string_new(&name);
	api->godot_string_parse_utf8(&name, anim_name);
	const void *c_args[] = {
		&name
	};

	api->godot_method_bind_ptrcall(mb, animated_sprite, c_args, NULL);
	api->godot_string_destroy(&name);
}

void play(godot_object *animated_sprite) {
	char *class_name = "AnimatedSprite";
	char *method_name = "play";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	godot_string name;
	api->godot_string_new(&name);
	api->godot_string_parse_utf8(&name, "");
	const void *c_args[] = {
		&name
	};

	api->godot_method_bind_ptrcall(mb, animated_sprite, c_args, NULL);
	api->godot_string_destroy(&name);
}