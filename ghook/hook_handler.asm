extern hook_handler_c:proc


.code 

;被hook的API会跳转到这
hook_handler_asm proc
	sub rsp,28h

	;保存一下易变参数
	push rcx
	push rdx
	push r8
	push r9
	push r15		;hook_handler_c会不会覆盖r15寄存器呢？

	sub rsp,28h
	call hook_handler_c
	add rsp,28h

	;恢复易变参数
	pop r15
	pop r9
	pop r8
	pop rdx
	pop rcx

	call r15
	add rsp,28h
	ret
hook_handler_asm endp




















end