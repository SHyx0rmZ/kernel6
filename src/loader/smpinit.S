.global smp_initializer_begin
smp_initializer_begin:

.incbin "build/out/smpinit"

.global smp_initializer_end
smp_initializer_end:
