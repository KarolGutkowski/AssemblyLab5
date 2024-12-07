.CODE
;                               rcx             rdx           r8         r9
; extern "C" void mul_m44_v4(float A[4][4], float v[][4], float r[][4], int n);
mul_m44_v4 proc
	xor		r10, r10		; petla iteracji
petla:
	cmp		r10, r9
	je		koniec
	xorps	xmm3, xmm3
	
	movaps	xmm1, dword ptr [rdx]

	vdpps	xmm2, xmm1, [rcx], 0F1H
	addps	xmm3, xmm2

	vdpps	xmm2, xmm1, [rcx+16], 0F2H
	addps	xmm3, xmm2

	vdpps	xmm2, xmm1, [rcx+32], 0F4H
	addps	xmm3, xmm2

	vdpps	xmm2, xmm1, [rcx+48], 0F8H
	addps	xmm3, xmm2

	movaps	[r8], xmm3

	add		rdx, 16
	add		r8, 16
	add		r10, 1
	jmp		petla
koniec:
	ret
mul_m44_v4 endp
;                                  rcx           rdx              r8 
; extern "C" void mul_m44_m44(float A[4][4], float B[4][4], float R[4][4]);
mul_m44_m44 proc
	xor		r10, r10	; licznik petli

	movaps	xmm5, [rdx]
	movaps	xmm6, [rdx+16]
	movaps	xmm7, [rdx+32]
	movaps	xmm8, [rdx+48]

petla:
	cmp		r10, 4
	je		koniec

	xorpd	xmm3, xmm3	; wyzerowanie xmm3
	
	VBROADCASTSS	xmm0, dword ptr [rcx]
	VFMADD231PS		xmm3, xmm0, xmm5

	VBROADCASTSS	xmm0, dword ptr [rcx+4]
	VFMADD231PS		xmm3, xmm0, xmm6

	VBROADCASTSS	xmm0, dword ptr [rcx+8]
	VFMADD231PS		xmm3, xmm0, xmm7

	VBROADCASTSS	xmm0, dword ptr [rcx+12]
	VFMADD231PS		xmm3, xmm0, xmm8

	movaps			[r8], xmm3

	add		rcx, 16
	add		r8, 16
	add		r10, 1
	jmp		petla
koniec:
	ret
mul_m44_m44 endp

end