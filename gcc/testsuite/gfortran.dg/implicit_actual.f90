! { dg-do compile }
! { dg-options "-O0" }
! Tests patch for problem that was found whilst investigating
! PR24158. The call to foo would cause an ICE because the
! actual argument was of a type that was not defined.
!
! Contributed by Paul Thomas <pault@gcc.gnu.org>
!
module global
  type :: t2
    type(t3), pointer :: d
  end type t2
end module global

program snafu
  use global
  implicit type (t3) (z)

  call foo (zin) ! { dg-error "defined|Type/rank" }

contains

  subroutine foo (z)

    type :: t3
      integer :: i
    end type t3

    type(t3)  :: z
    z%i = 1

  end subroutine foo
end program snafu

