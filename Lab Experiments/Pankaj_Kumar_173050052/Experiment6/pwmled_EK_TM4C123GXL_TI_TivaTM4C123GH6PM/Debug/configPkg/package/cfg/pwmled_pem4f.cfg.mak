# invoke SourceDir generated makefile for pwmled.pem4f
pwmled.pem4f: .libraries,pwmled.pem4f
.libraries,pwmled.pem4f: package/cfg/pwmled_pem4f.xdl
	$(MAKE) -f E:\TIworkspace\pwmled_EK_TM4C123GXL_TI_TivaTM4C123GH6PM/src/makefile.libs

clean::
	$(MAKE) -f E:\TIworkspace\pwmled_EK_TM4C123GXL_TI_TivaTM4C123GH6PM/src/makefile.libs clean

