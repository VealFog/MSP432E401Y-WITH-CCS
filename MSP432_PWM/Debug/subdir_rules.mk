################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ccs/ccsv8/tools/compiler/ti-cgt-arm_18.1.8.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ccscode/MSP432_PWM" --include_path="D:/ccs/simplelink_msp432e4_sdk_4_20_00_12/source" --include_path="D:/ccs/simplelink_msp432e4_sdk_4_20_00_12/source/third_party/CMSIS/Include" --include_path="D:/ccs/simplelink_msp432e4_sdk_4_20_00_12/source/ti/net/bsd" --include_path="D:/ccs/ccsv8/tools/compiler/ti-cgt-arm_18.1.8.LTS/include" --advice:power=none --define=DeviceFamily_MSP432E4 --define=__MSP432E401Y__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

system_msp432e401y.obj: ../system_msp432e401y.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ccs/ccsv8/tools/compiler/ti-cgt-arm_18.1.8.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ccscode/MSP432_PWM" --include_path="D:/ccs/simplelink_msp432e4_sdk_4_20_00_12/source" --include_path="D:/ccs/simplelink_msp432e4_sdk_4_20_00_12/source/third_party/CMSIS/Include" --include_path="D:/ccs/simplelink_msp432e4_sdk_4_20_00_12/source/ti/net/bsd" --include_path="D:/ccs/ccsv8/tools/compiler/ti-cgt-arm_18.1.8.LTS/include" --advice:power=none --define=DeviceFamily_MSP432E4 --define=__MSP432E401Y__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="system_msp432e401y.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


