extern hook_handler_c:proc


.code 

;��hook��API����ת����
hook_handler_asm proc
	sub rsp,28h

	;����һ���ױ����
	push rcx
	push rdx
	push r8
	push r9
	push r15		;hook_handler_c�᲻�Ḳ��r15�Ĵ����أ�

	sub rsp,28h
	call hook_handler_c
	add rsp,28h

	;�ָ��ױ����
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