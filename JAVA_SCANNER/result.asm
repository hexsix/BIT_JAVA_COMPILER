.while a<b
	mov		edx, 0
	mov		eax, 2
	cdq
	mov		ebx, 3
	idiv	ebx
	mov		ebx, eax

	imul	eax, ebx, 4
	mov		ebx, eax

	mov		eax, ebx
	add		eax, 1
	mov		ebx, eax

	mov		eax, 5
	sub		eax, ebx
	mov		ebx, eax

	mov		eax, ebx
	mov		a, eax
.endw