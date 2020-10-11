

                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                EXTERN  __iar_program_start
                EXTERN  main
                PUBLIC  __vector_table
    PUBLIC  __low_level_init
                
                SECTION .intvec:CODE:ROOT(8)
                DATA
__vector_table  
                DCD     sfe(CSTACK)               ; Top of Stack
                DCD     __iar_program_start             ; Reset
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault


                THUMB

                PUBWEAK NMI_Handler
                PUBWEAK HardFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(2)
                
NMI_Handler
                ;B       NMI_Handler

HardFault_Handler
                B       HardFault_Handler
                
__low_level_init:

                LDR  R1, =sfe(CSTACK)        ; restore original stack pointer
                MSR  MSP, R1
                LDR  r0,=0x20010000
                LDR  r1,=(0x600/4)      ;(8192/4)
                ldr  r2,=0x47704770
__lowLLable0:
                STR  r2,[R0]
                adds r0,r0,#4
                subs r1,r1,#1
                bne.n __lowLLable0
               
                MOVS  R0,#1
   ;; Return with BX to be independent of mode of caller
                BX   LR
               
                
                END
