extern "C" void hook_handler_asm();


extern "C" void hook_handler_c() {
	printf("[+]hook_handler \n");
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;