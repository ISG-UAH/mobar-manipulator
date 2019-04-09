(define (problem BlocksWorld1)
(:domain MoBAr-Arm)
 (:objects
    white green blue red - block
    P0_12 P10_12 P0_16 P-10_12 - pos
 )
 (:init
(arm-empty)
(arm-position P0_12)
(table-clear P0_12)
(position red P10_12)
(clear red)
(position white P-10_12)
(clear white)
(position green P-10_12)
(on green white)
  )
 (:goal
    (and
        (position green P10_12)
        (on green red)
        (on red white)
        (arm-position P0_12)
    )
 )
)
