
.code

VmExecute proc
	mov [rcx+900h],r12				;STORE REGISTERS WE USE
	pushfq
	pop rax
	mov [rcx+9F8h],rax				;STORE FLAGS

	mov rax,HandlerDone
	mov [rcx+100h],rax
	
InstructionLoop:
	mov rax,[rcx]					;load vip
	movzx rax,byte ptr[rax]
	jmp qword ptr[rcx+100h+rax*8h]	;jmp to instruction handler
HandlerDone:
	cmp qword ptr[rcx],0h			;if ip zero then break
	jnz InstructionLoop

	mov rax,[rcx+9F8h]
	push rax
	popfq
	mov r12,[rcx+900h]
	ret
VmExecute endp

ViNop proc
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViNop endp
	
ViEndOfCode proc
	mov qword ptr[rcx],0h
	jmp qword ptr[rcx+100h]
ViEndOfCode endp

ViLdib proc
	mov rax,[rcx]
	mov al,byte ptr[rax+1]
	sub rsp,1h
	mov byte ptr[rsp],al
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdib endp

ViLdiw proc
	mov rax,[rcx]
	mov ax,word ptr[rax+1]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdiw endp

ViLdid proc
	mov rax,[rcx]
	mov eax,dword ptr[rax+1]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],5h
	jmp qword ptr[rcx+100h]
ViLdid endp

ViLdiq proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1]
	push rax
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViLdiq endp

ViLdibsxw proc
	mov rax,[rcx]
	movsx ax,byte ptr[rax+1]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdibsxw endp

ViLdibsxd proc
	mov rax,[rcx]
	movsx eax,byte ptr[rax+1]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdibsxd endp

ViLdibsxq proc
	mov rax,[rcx]
	movsx rax,byte ptr[rax+1]
	sub rsp,8h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdibsxq endp

ViLdiwsxd proc
	mov rax,[rcx]
	movsx eax,word ptr[rax+1]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdiwsxd endp

ViLdiwsxq proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	sub rsp,8h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdiwsxq endp

ViLdidsxq proc
	mov rax,[rcx]
	movsxd rax,dword ptr[rax+1]
	push rax
	add qword ptr[rcx],5h
	jmp qword ptr[rcx+100h]
ViLdidsxq endp

ViLdibzxw proc
	mov rax,[rcx]
	movzx ax,byte ptr[rax+1]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdibzxw endp

ViLdibzxd proc
	mov rax,[rcx]
	movzx eax,byte ptr[rax+1]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdibzxd endp

ViLdibzxq proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1]
	sub rsp,8h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdibzxq endp

ViLdiwzxd proc
	mov rax,[rcx]
	movzx eax,word ptr[rax+1]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdiwzxd endp

ViLdiwzxq proc
	mov rax,[rcx]
	movzx rax,word ptr[rax+1]
	sub rsp,8h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdiwzxq endp

ViLdidzxq proc
	mov rax,[rcx]
	mov eax,dword ptr[rax+1]
	push rax
	add qword ptr[rcx],5h
	jmp qword ptr[rcx+100h]
ViLdidzxq endp

ViLdab proc
	mov rax,qword ptr[rsp]
	mov al,byte ptr[rax]
	sub rsp,1h
	mov byte ptr[rsp],al
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViLdab endp

ViLdaw proc
	mov rax,qword ptr[rsp]
	mov ax,word ptr[rax]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViLdaw endp

ViLdad proc
	mov rax,qword ptr[rsp]
	mov eax,dword ptr[rax]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViLdad endp

ViLdaq proc
	mov rax,qword ptr[rsp]
	push qword ptr[rax]
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViLdaq endp

ViStab proc
	mov al,byte ptr[rsp]
	add rsp,1h
	mov r12,qword ptr[rsp]
	mov byte ptr[r12],al
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViStab endp

ViStaw proc
	mov ax,word ptr[rsp]
	add rsp,2h
	mov r12,qword ptr[rsp]
	mov word ptr[r12],ax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViStaw endp

ViStad proc
	mov eax,dword ptr[rsp]
	add rsp,4h
	mov r12,qword ptr[rsp]
	mov dword ptr[r12],eax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViStad endp

ViStaq proc
	pop rax
	mov r12,qword ptr[rsp]
	mov qword ptr[r12],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViStaq endp

ViSxbw proc
	mov al,byte ptr[rsp]
	movsx ax,al
	sub rsp,1h
	mov word ptr[rsp],ax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSxbw endp

ViSxbd proc
	mov al,byte ptr[rsp]
	movsx eax,al
	sub rsp,3h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSxbd endp

ViSxbq proc
	mov al,byte ptr[rsp]
	movsx rax,al
	sub rsp,7h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSxbq endp

ViSxwd proc
	mov ax,word ptr[rsp]
	movsx eax,ax
	sub rsp,2h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSxwd endp

ViSxwq proc
	mov ax,word ptr[rsp]
	movsx rax,ax
	sub rsp,6h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSxwq endp

ViSxdq proc
	mov eax,dword ptr[rsp]
	movsxd rax,eax
	sub rsp,4h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSxdq endp

ViZxbw proc
	mov al,byte ptr[rsp]
	movzx ax,al
	sub rsp,1h
	mov word ptr[rsp],ax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViZxbw endp

ViZxbd proc
	mov al,byte ptr[rsp]
	movzx eax,al
	sub rsp,3h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViZxbd endp

ViZxbq proc
	mov al,byte ptr[rsp]
	movzx rax,al
	sub rsp,7h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViZxbq endp

ViZxwd proc
	mov ax,word ptr[rsp]
	movzx eax,ax
	sub rsp,2h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViZxwd endp

ViZxwq proc
	mov ax,word ptr[rsp]
	movzx rax,ax
	sub rsp,6h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViZxwq endp

ViZxdq proc
	mov eax,dword ptr[rsp]
	sub rsp,4h
	mov qword ptr[rsp],rax
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViZxdq endp

ViPushb proc
	mov rax,[rcx]					;load vip
	movzx rax,byte ptr[rax+1h]		;zero extend regid
	mov al,byte ptr[rcx+rax*8h]	;load btye from register
	sub rsp,1h
	mov byte ptr[rsp],al
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPushb endp

ViPushw proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov ax,word ptr[rcx+rax*8h]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPushw endp

ViPushd proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov eax,dword ptr[rcx+rax*8h]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPushd endp

ViPushq proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	push qword ptr[rcx+rax*8h]
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPushq endp

ViPopb proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov r12b,byte ptr[rsp]
	add rsp,1h
	mov byte ptr[rcx+rax*8h],r12b
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPopb endp

ViPopw proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov r12w,word ptr[rsp]
	add rsp,2h
	mov word ptr[rcx+rax*8h],r12w
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPopw endp

ViPopd proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov r12d,dword ptr[rsp]
	add rsp,4h
	mov dword ptr[rcx+rax*8h],r12d
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPopd endp

ViPopq proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	pop r12
	mov qword ptr[rcx+rax*8h],r12
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViPopq endp

ViNpopb proc
	add rsp,1
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViNpopb endp

ViNpopw proc
	add rsp,2
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViNpopw endp

ViNpopd proc
	add rsp,4
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViNpopd endp

ViNpopq proc
	add rsp,8
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViNpopq endp

ViAddb proc
	mov al,byte ptr[rsp]
	add rsp,1h
	add byte ptr[rsp],al
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAddb endp

ViAddw proc
	mov ax,word ptr[rsp]
	add rsp,2h
	add word ptr[rsp],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAddw endp

ViAddd proc
	mov eax,dword ptr[rsp]
	add rsp,4h
	add dword ptr[rsp],eax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAddd endp

ViAddq proc
	pop rax
	add qword ptr[rsp],rax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAddq endp

ViSubb proc
	mov al,byte ptr[rsp]
	add rsp,1h
	sub byte ptr[rsp],al
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSubb endp

ViSubw proc
	mov ax,word ptr[rsp]
	add rsp,2h
	sub word ptr[rsp],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSubw endp

ViSubd proc
	mov eax,dword ptr[rsp]
	add rsp,4h
	sub dword ptr[rsp],eax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSubd endp

ViSubq proc
	pop rax
	sub qword ptr[rsp],rax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSubq endp

ViMulb proc
	mov al,[rsp]
	mul byte ptr[rsp+1h]
	mov [rsp],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViMulb endp

ViMulw proc
	mov ax,[rsp]
	mul word ptr[rsp+2h]
	mov [rsp+2h],ax
	mov [rsp],dx
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViMulw endp

ViMuld proc
	mov eax,[rsp]
	mul dword ptr[rsp+4h]
	mov [rsp+4h],eax
	mov [rsp],edx
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViMuld endp

ViMulq proc
	mov rax,[rsp]
	mul qword ptr[rsp+8h]
	mov [rsp+8h],rax
	mov [rsp],rdx
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViMulq endp

ViDivb proc
	mov ax,[rsp+1h]
	div byte ptr[rsp]
	add rsp,1h
	mov [rsp+1h],al
	mov [rsp],ah
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDivb endp

ViDivw proc
	mov ax,[rsp+4h]
	mov dx,[rsp+2h]
	div word ptr[rsp]
	add rsp,2h
	mov [rsp+2h],ax
	mov [rsp],dx
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDivw endp

ViDivd proc
	mov eax,[rsp+8h]
	mov edx,[rsp+4h]
	div dword ptr[rsp]
	add rsp,4h
	mov [rsp+4h],eax
	mov [rsp],edx
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDivd endp

ViDivq proc
	mov rax,[rsp+10h]
	mov rdx,[rsp+8h]
	div qword ptr[rsp]
	add rsp,8h
	mov [rsp+8h],rax
	mov [rsp],rdx
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDivq endp

ViAndb proc
	mov al,byte ptr[rsp]
	add rsp,1h
	and byte ptr[rsp],al
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAndb endp

ViAndw proc
	mov ax,word ptr[rsp]
	add rsp,2h
	and word ptr[rsp],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAndw endp

ViAndd proc
	mov eax,dword ptr[rsp]
	add rsp,4h
	and dword ptr[rsp],eax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAndd endp

ViAndq proc
	pop rax
	and qword ptr[rsp],rax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViAndq endp

ViOrb proc
	mov al,byte ptr[rsp]
	add rsp,1h
	or byte ptr[rsp],al
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViOrb endp

ViOrw proc
	mov ax,word ptr[rsp]
	add rsp,2h
	or word ptr[rsp],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViOrw endp

ViOrd proc
	mov eax,dword ptr[rsp]
	add rsp,4h
	or dword ptr[rsp],eax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViOrd endp

ViOrq proc
	pop rax
	or qword ptr[rsp],rax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViOrq endp

ViXorb proc
	mov al,byte ptr[rsp]
	add rsp,1h
	xor byte ptr[rsp],al
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViXorb endp

ViXorw proc
	mov ax,word ptr[rsp]
	add rsp,2h
	xor word ptr[rsp],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViXorw endp

ViXord proc
	mov eax,dword ptr[rsp]
	add rsp,4h
	xor dword ptr[rsp],eax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViXord endp

ViXorq proc
	pop rax
	xor qword ptr[rsp],rax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViXorq endp

ViShlb proc
	mov r12,rcx
	mov cl,byte ptr[rsp]	;load shift amout
	add rsp,1h
	shl byte ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShlb endp

ViShlw proc
	mov r12,rcx
	mov cl,byte ptr[rsp]
	add rsp,1h
	shl word ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShlw endp

ViShld proc
	mov r12,rcx
	mov cl,byte ptr[rsp]
	add rsp,1h
	shl dword ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShld endp

ViShlq proc
	mov r12,rcx
	mov cl,byte ptr[rsp]
	add rsp,1h
	shl qword ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShlq endp

ViShrb proc
	mov r12,rcx
	mov cl,byte ptr[rsp]	;load shift amout
	add rsp,1h
	shr byte ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShrb endp

ViShrw proc
	mov r12,rcx
	mov cl,byte ptr[rsp]
	add rsp,1h
	shr word ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShrw endp

ViShrd proc
	mov r12,rcx
	mov cl,byte ptr[rsp]
	add rsp,1h
	shr dword ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShrd endp

ViShrq proc
	mov r12,rcx
	mov cl,byte ptr[rsp]
	add rsp,1h
	shr qword ptr[rsp],cl
	pushfq
	mov rcx,r12
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViShrq endp

ViCmpb proc
	mov al,byte ptr[rsp]
	cmp byte ptr[rsp+1],al
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViCmpb endp

ViCmpw proc
	mov ax,word ptr[rsp]
	cmp word ptr[rsp+2],ax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViCmpw endp

ViCmpd proc
	mov eax,dword ptr[rsp]
	cmp dword ptr[rsp+4],eax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViCmpd endp

ViCmpq proc
	mov rax,qword ptr[rsp]
	cmp qword ptr[rsp+8],rax
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViCmpq endp

ViJmp proc
	pop rax
	add [rcx],rax
	jmp qword ptr[rax+100h]
ViJmp endp

ViJmpi proc
	mov rax,[rcx]				;load ip
	mov rax,[rax+1h]			;load 8 byte immediate jump offset
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJmpi endp

ViJl proc
	popfq					;pop rflags
	jl DoJump
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
DoJump:
	mov rax,[rcx]			;load ip
	mov rax,[rax+1h]		;load 8 byte immediate jump offset
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJl endp

ViJle proc
	popfq
	jle DoJump
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
DoJump:
	mov rax,[rcx]
	mov rax,[rax+1h]
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJle endp

ViJg proc
	popfq
	jg DoJump
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
DoJump:
	mov rax,[rcx]
	mov rax,[rax+1h]
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJg endp

ViJge proc
	popfq
	jge DoJump
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
DoJump:
	mov rax,[rcx]
	mov rax,[rax+1h]
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJge endp

ViJz proc
	popfq
	jz DoJump
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
DoJump:
	mov rax,[rcx]
	mov rax,[rax+1h]
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJz endp

ViJnz proc
	popfq
	jnz DoJump
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
DoJump:
	mov rax,[rcx]
	mov rax,[rax+1h]
	add [rcx],rax
	jmp qword ptr[rcx+100h]
ViJnz endp

ViCall proc
	mov rax,[rcx]
	mov r12,[rax+1h]			;load jump delta from instruction imm
	add rax,9h
	push rax
	add [rcx],r12
	jmp qword ptr[rcx+100h]
ViCall endp

ViRet proc
	pop [rcx]				;pop value from top of stack into vip
	jmp qword ptr[rcx+100h]
ViRet endp

ViX86Call proc
	mov r12,rcx				;preserve rcx
	mov r9,[rsp+20h]
	mov r8,[rsp+18h]
	mov rdx,[rsp+10h]
	mov rcx,[rsp+8h]
	pop rax					;pop call address from stack
	call rax				;call it omegalawl(large(big(sizeable)))
	push rax				;push the value it returns
	mov rcx,r12				;restore rcx
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViX86Call endp

ViEditsp proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1h]
	add rsp,rax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViEditsp endp

ViStsp proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov [rcx+rax*8h],rsp
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViStsp endp

ViLdsp proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov rsp,[rcx+rax*8h]
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdsp endp

ViStip proc
	mov rax,[rcx]
	mov r12,rax
	movzx rax,byte ptr[rax+1h]
	mov [rcx+rax*8h],r12
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViStip endp

ViLdip proc
	mov rax,[rcx]
	movzx rax,byte ptr[rax+1h]
	mov rax,[rcx+rax*8h]
	mov [rcx],rax
	add qword ptr[rcx],2h
	jmp qword ptr[rcx+100h]
ViLdip endp

ViSwapb proc
	mov r8b,byte ptr[rsp]
	mov r9b,byte ptr[rsp+1h]
	mov byte ptr[rsp+1h],r8b
	mov byte ptr[rsp],r9b
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapb endp

ViSwapw proc
	mov r8w,word ptr[rsp]
	mov r9w,word ptr[rsp+2h]
	mov word ptr[rsp+2h],r8w
	mov word ptr[rsp],r9w
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapw endp

ViSwapd proc
	mov r8d,dword ptr[rsp]
	mov r9d,dword ptr[rsp+4h]
	mov dword ptr[rsp+4h],r8d
	mov dword ptr[rsp],r9d
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapd endp

ViSwapq proc
	mov r8,qword ptr[rsp]
	mov r9,qword ptr[rsp+8h]
	mov qword ptr[rsp+8h],r8
	mov qword ptr[rsp],r9
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapq endp

ViDecb proc
	sub byte ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDecb endp

ViDecw proc
	sub word ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDecw endp

ViDecd proc
	sub dword ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDecd endp

ViDecq proc
	sub qword ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViDecq endp

ViIncb proc
	add byte ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViIncb endp

ViIncw proc
	add word ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViIncw endp

ViIncd proc
	add dword ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViIncd endp

ViIncq proc
	add qword ptr[rsp],1h
	pushfq
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViIncq endp

ViSwapbq proc
	mov rax,qword ptr[rsp]
	mov r8b,byte ptr[rsp+8h]
	mov qword ptr[rsp+1h],rax
	mov byte ptr[rsp],r8b
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapbq endp

ViSwapwq proc
	mov rax,qword ptr[rsp]
	mov r8w,word ptr[rsp+8h]
	mov qword ptr[rsp+2h],rax
	mov word ptr[rsp],r8w
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapwq endp

ViSwapdq proc
	mov rax,qword ptr[rsp]
	mov r8d,dword ptr[rsp+8h]
	mov qword ptr[rsp+4h],rax
	mov dword ptr[rsp],r8d
	add qword ptr[rcx],1h
	jmp qword ptr[rcx+100h]
ViSwapdq endp

ViLdsb proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov al,byte ptr[rsp+rax]
	sub rsp,1h
	mov byte ptr[rsp],al
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViLdsb endp

ViLdsw proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov ax,word ptr[rsp+rax]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViLdsw endp

ViLdsd proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov eax,dword ptr[rsp+rax]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViLdsd endp

ViLdsq proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	push qword ptr[rsp+rax]
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViLdsq endp

ViStsb proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov r12b,byte ptr[rsp]
	mov byte ptr[rsp+rax],r12b
	add rsp,1h
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViStsb endp

ViStsw proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov r12w,word ptr[rsp]
	mov word ptr[rsp+rax],r12w
	add rsp,2h
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViStsw endp

ViStsd proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov r12d,dword ptr[rsp]
	mov dword ptr[rsp+rax],r12d
	add rsp,4h
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViStsd endp

ViStsq proc
	mov rax,[rcx]
	mov rax,qword ptr[rax+1h]
	mov r12,qword ptr[rsp]
	mov qword ptr[rsp+rax],r12
	add rsp,8h
	add qword ptr[rcx],9h
	jmp qword ptr[rcx+100h]
ViStsq endp

ViLdbpb proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	mov al,byte ptr[rax]
	sub rsp,1h
	mov byte ptr[rsp],al
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdbpb endp

ViLdbpw proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	mov ax,word ptr[rax]
	sub rsp,2h
	mov word ptr[rsp],ax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdbpw endp

ViLdbpd proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	mov eax,dword ptr[rax]
	sub rsp,4h
	mov dword ptr[rsp],eax
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdbpd endp

ViLdbpq proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	push qword ptr[rax]
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViLdbpq endp

ViStbpb proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	mov r12b,[rsp]
	add rsp,1h
	mov [rax],r12b
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViStbpb endp

ViStbpw proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	mov r12w,[rsp]
	add rsp,2h
	mov [rax],r12w
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViStbpw endp

ViStbpd proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	mov r12d,[rsp]
	add rsp,4h
	mov [rax],r12d
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViStbpd endp

ViStbpq proc
	mov rax,[rcx]
	movsx rax,word ptr[rax+1]
	add rax,[rcx+0F8h]
	pop [rax]
	add qword ptr[rcx],3h
	jmp qword ptr[rcx+100h]
ViStbpq endp

end