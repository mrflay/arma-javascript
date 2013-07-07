/*
	Copyright (C) 2013 Simas Toleikis

	JavaScript addon macros and constants.
*/

// All JavaScript extension protocol commands start with a "#" symbol.
// The protocol command is then followed by a single-byte token with
// optional extra payload.
#define JS_PROTOCOL_COMMAND '#'
#define JS_PROTOCOL_LENGTH 2

// Extension protocol command tokens
#define JS_PROTOCOL_TOKEN_INIT 'I'
#define JS_PROTOCOL_TOKEN_SPAWN 'S'
#define JS_PROTOCOL_TOKEN_TERMINATE 'T'
#define JS_PROTOCOL_TOKEN_DONE 'D'
#define JS_PROTOCOL_TOKEN_VERSION 'V'

// Full command strings (for SQF)
#define JS_PROTOCOL_COMMAND_INIT "#I"
#define JS_PROTOCOL_COMMAND_SPAWN "#S"
#define JS_PROTOCOL_COMMAND_TERMINATE "#T"
#define JS_PROTOCOL_COMMAND_DONE "#D"
#define JS_PROTOCOL_COMMAND_VERSION "#V"

// Macro-based JavaScript code execution
#define JS(CODE) (call compile ("JavaScript" callExtension ##CODE##))