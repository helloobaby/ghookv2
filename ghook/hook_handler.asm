extern hook_handler_c:proc


.code 

;��hook��API����ת����
hook_handler_asm proc

	;int 3
	sub rsp,28h			;hook_handler_c�в�����,���������ջ28h�ֽ�
	call hook_handler_c
	add rsp,28h

	ret
hook_handler_asm endp




















end