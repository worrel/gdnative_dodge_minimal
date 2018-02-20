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
void set_animation(godot_object *animated_sprite, char *anim_name);
godot_node_path get_node_path(godot_object *instance);
godot_object *get_node(godot_object *instance, char *node_name);
godot_string get_node_name(godot_object *instance);

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
	// commenting out, no user data right now
	//api->godot_free(p_user_data);
}

godot_variant ready(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;

	printf( "ready!\n" );

	{
		godot_node_path player_path = get_node_path(p_instance);
		godot_string player_path_gstring = api->godot_node_path_as_string(&player_path);
		godot_char_string player_path_cstring = api->godot_string_utf8(&player_path_gstring);
		printf("Player path= %s\n", api->godot_char_string_get_data(&player_path_cstring));

		api->godot_string_destroy(&player_path_gstring);
		api->godot_char_string_destroy(&player_path_cstring);
		api->godot_node_path_destroy(&player_path);
	}

	{
		godot_string node_name = get_node_name(p_instance);
		godot_char_string node_name_cstring = api->godot_string_utf8(&node_name);
		printf("Node name== %s\n", api->godot_char_string_get_data(&node_name_cstring));

		api->godot_string_destroy(&node_name);
		api->godot_char_string_destroy(&node_name_cstring);
	}

	godot_object* as = get_node(p_instance, "AnimatedSprite");

	set_animation(as, "up");

	api->godot_object_destroy(as);

	return ret;
}

godot_variant process(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	//printf( "process\n" );
	
	{
		godot_node_path player_path = get_node_path(p_instance);
		godot_string player_path_gstring = api->godot_node_path_as_string(&player_path);
		godot_char_string player_path_cstring = api->godot_string_utf8(&player_path_gstring);
		//printf("Player path= %s\n", api->godot_char_string_get_data(&player_path_cstring));

		api->godot_string_destroy(&player_path_gstring);
		api->godot_char_string_destroy(&player_path_cstring);
		api->godot_node_path_destroy(&player_path);
	}

	{
		godot_string node_name = get_node_name(p_instance);

		godot_char_string node_name_cstring = api->godot_string_utf8(&node_name);
		//printf("Node name== %s\n", api->godot_char_string_get_data(&node_name_cstring));

		api->godot_string_destroy(&node_name);
		api->godot_char_string_destroy(&node_name_cstring);
	}

	return ret;
}

godot_string get_node_name(godot_object *instance) {
	char *class_name = "Node";
	char *method_name = "get_name";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	godot_string nm;
	const void *c_args[1] = {};

	api->godot_method_bind_ptrcall(mb, instance, c_args, &nm);

	return nm;
}

godot_node_path get_node_path(godot_object *instance) {
	char *class_name = "Node";
	char *method_name = "get_path";

	static godot_method_bind *mb = NULL;
	if (!mb) {
		mb = api->godot_method_bind_get_method(class_name, method_name);
	}

	godot_node_path np;
	const void *c_args[1] = {};

	api->godot_method_bind_ptrcall(mb, instance, c_args, &np);
	return np;
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