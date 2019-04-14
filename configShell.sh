!
interface ens224
 ip address 100.2.4.4/24
 q
!
interface lo
 ip address 4.4.4.4/32
 q
!
interface ens192 vrf left
 ip address 100.4.6.4/24
 q
!
router ospf
 ospf router-id 4.4.4.4
 network 4.4.4.4/32 area 0
 network 100.2.4.0/24 area 0
 q
!
mpls ldp
 router-id 4.4.4.4
 !
 address-family ipv4
  discovery transport-address 4.4.4.4
  !
  interface ens224
  !
 exit-address-family
 q
 !
!
router bgp 100
 bgp router-id 4.4.4.4
 neighbor 5.5.5.5 remote-as 100
 neighbor 5.5.5.5 update-source 4.4.4.4
 !
 address-family ipv4 vpn
  neighbor 5.5.5.5 activate
 exit-address-family
!
