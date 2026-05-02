# Common compilation rules for Lanex OS
# Must be included AFTER defining: OBJ_DIR, SRC_DIR, MSG_PREFIX, CFLAGS_USER
# Optional: EXTRA_DEP (additional prerequisite for .c objects)

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c $(EXTRA_DEP)
	@echo "  CC       $(MSG_PREFIX)/$<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_USER) -c $< -o $@

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s
	@echo "  ASM      $(MSG_PREFIX)/$<"
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@echo "  ASM      $(MSG_PREFIX)/$<"
	@mkdir -p $(dir $@)
	@$(NASM) -f elf32 $< -o $@
