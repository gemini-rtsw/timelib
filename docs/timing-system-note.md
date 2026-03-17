# Timing System Note (Consolidated)

This note has been consolidated into the formal design change document:

- `docs/design-change-bancomm-removal-ntp.md`

Use that document as the authoritative source for:
- Bancomm removal scope and decision record.
- NTP-primary synchronization across TCS, MCS, CRCS, SCS, PCS, and A&G.
- RTEMS 6.2 `libbsd` + `rtems-net-services` `ntpd` implementation details.
- GPS->UTC->TAI timing model and timelib UTC->TAI normalization invariants.
