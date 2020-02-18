################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ADC_Task.obj: ../ADC_Task.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="ADC_Task.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Control_Task.obj: ../Control_Task.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="Control_Task.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

State_Task.obj: ../State_Task.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="State_Task.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Yaw_Task.obj: ../Yaw_Task.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="Yaw_Task.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

buttons4.obj: ../buttons4.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="buttons4.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

display.obj: ../display.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="display.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos_demo.obj: ../freertos_demo.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="freertos_demo.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pwmGen.obj: ../pwmGen.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="pwmGen.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="P:/Courses/ENCE464/Heli_FreeRTOS/freertos_demo" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB2 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


