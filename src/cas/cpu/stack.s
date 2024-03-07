.global _set_stack_ptr
.global _get_stack_ptr

.align 2
_set_stack_ptr:
  mov r4, r15
  rts
  mov r4, r0

_get_stack_ptr:
  mov r15, r0
  rts
  nop
