.text
.section .text

.global rsp_break
.ent rsp_break 
.type rsp_break, @function
rsp_break:
	break
	jr $ra
.end rsp_break
.size rsp_break, .-rsp_break

.global rsp_debug_break
.ent rsp_debug_break 
.type rsp_debug_break, @function
rsp_debug_break:
	break
	jr $ra
.end rsp_debug_break
.size rsp_debug_break, .-rsp_debug_break
