extern hook_handler_c:proc


.code 

;被hook的API会跳转到这
hook_handler_asm proc

	;int 3
	sub rsp,28h			;hook_handler_c有参数的,保险起见退栈28h字节
	call hook_handler_c
	add rsp,28h

	ret
hook_handler_asm endp




















end