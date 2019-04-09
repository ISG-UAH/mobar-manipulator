(define (problem BlocksWorld1)
(:domain MoBAr-Arm)
 (:objects
    white green blue red - block
    P10_15 P0_18 P-10_15 P10_10 - pos
 )
 (:init
(arm-empty)
(arm-position P0_18)
(table-clear P0_18)
(position red P10_15)
(clear red)
(position white P0_18)
(clear white)
(position green P-10_15)
(position blue P10_10)
(on green white)
