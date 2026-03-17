# Design Change Document: Bancomm Removal and NTP-Primary Time Synchronization

## Document Control
- **Document ID:** TBD
- **Project:** timelib
- **Status:** Draft for review
- **Author:** Matt Rippa
- **Date:** 2025-12-05
- **Related Commit:** `7e5a7ec41dfc563c2979df24e53a2fa0b75a68a1` (`garch-23: Remove Bancomm time provider support`)

## Reference Documents
| Reference | Location |
| --- | --- |
| RD-001: RTEMS 6.2 Release Notes | [rtems.org/r-6-2](https://ftp.rtems.org/pub/rtems/releases/6/6.2/rtems-6.2-release-notes/r-6-2.html) |
| RD-002: EPICS Base generalTime Framework | [epicsGeneralTime.h](https://docs.epics-controls.org/projects/base/en/latest/epicsGeneralTime_h.html) |
| RD-003: Bancomm Removal Commit (garch-23) | [timelib commit 7e5a7ec](https://gitlab.com/nsf-noirlab/gemini/rtsw/support/timelib/-/commit/7e5a7ec41dfc563c2979df24e53a2fa0b75a68a1) |
| RD-004: Leap-Second Detailed Procedures | docs/Leap+Seconds.md |
| RD-005: RTEMS Net Services Build | [RTEMS rtems-net-services repository](https://github.com/RTEMS/rtems-net-services) |
| RD-006: RTEMS libbsd Build Context | [RTEMS rtems-libbsd repository](https://github.com/RTEMS/rtems-libbsd) |
| RD-007: RTEMS NTP Source Import Map | [ntp-file-import.json (main)](https://github.com/RTEMS/rtems-net-services/blob/main/ntp-file-import.json) |
| RD-008: RTEMS ntpd API Header | [rtems/ntpd.h (main)](https://github.com/RTEMS/rtems-net-services/blob/main/bsd/rtemsbsd/include/rtems/ntpd.h) |
| RD-009: RTEMS ntpd Integration/Test Example | [testsuites/ntp01/test_main.c (main)](https://github.com/RTEMS/rtems-net-services/blob/main/testsuites/ntp01/test_main.c) |
| RD-010: Consolidated Timing Summary Note | docs/timing-system-note.md |
| RD-011: Module Release History | RELEASE.NOTES |
| RD-012: IERS Bulletin C Latest Issue Page | [IERS Bulletin C news page](https://www.iers.org/SharedDocs/News/EN/BulletinC) |
| RD-013: IERS Bulletin C Latest Text (machine-readable) | [latestVersion/bulletinC.txt](https://datacenter.iers.org/data/latestVersion/bulletinC.txt) |
| RD-014: IERS Bulletin C Archive (all versions) | [availableVersions.php?id=16](https://datacenter.iers.org/availableVersions.php?id=16) |

## 1. Purpose
This document formalizes the design change that removes Bancomm hardware module dependencies from timelib and establishes Network Time Protocol (NTP) as the primary time synchronization mechanism across core telescope systems: TCS, MCS, CRCS, SCS, PCS, and A&G.

## 2. Background
Historically, observatory timing support included Bancomm/IRIG-B specific code paths and weak-symbol compatibility aliases. This created additional maintenance overhead and dual-path behavior tied to legacy hardware assumptions.

The `garch-23` change removes Bancomm-specific support in timelib and standardizes the time acquisition path through EPICS `generalTime`, consistent with Linux and RTEMS NTP-disciplined clocks [RD-002][RD-003].

## 3. Scope
### In Scope
- Removal of Bancomm/IRIG-B-specific timelib source usage and weak aliases.
- Consolidation of current-time acquisition through EPICS `generalTime`.
- Definition of NTP-disciplined system clocks as the primary synchronization source for:
  - TCS
  - MCS
  - CRCS
  - SCS
  - PCS
  - A&G
- Preservation of timelib UTC-to-TAI normalization behavior.

### Out of Scope
- Changes to telescope control algorithms not related to time source selection.
- Replacement or migration planning for external legacy systems not part of the six core systems listed above.
- Network architecture redesign beyond existing NTP distribution from the observatory timing source.

## 4. Design Decision
### Decision Statement
All core telescope systems (TCS, MCS, CRCS, SCS, PCS, A&G) shall use NTP-synchronized system time as the primary reference. timelib shall obtain current time via EPICS `generalTime` provider selection and shall not include Bancomm-specific code paths.

### Rationale
- Reduces platform-specific branching and hardware-coupled logic.
- Aligns all core systems on a common timing protocol (NTP over Ethernet) and operational model.
- Improves maintainability and testability of timelib.
- Retains established UTC->TAI conversion semantics required by downstream time-scale calculations.

## 5. Functional Architecture
1. Observatory timing source provides authoritative reference time.
2. NTP distributes time to host OS clocks across TCS, MCS, CRCS, SCS, PCS, and A&G.
3. EPICS `generalTime` reads the disciplined OS clock and exposes UTC to applications [RD-002].
4. timelib converts UTC to TAI using configured leap-second offset (`datlsd`) and existing epoch handling.
5. Higher-level conversions (UTC/UT1/TT/TDB/sidereal outputs) continue from the TAI baseline.

### 5.1 RTEMS 6.2 NTP Implementation (libbsd + net-services)
For RTEMS 6.2 systems (MCS, CRCS, SCS, PCS, A&G), `ntpd` is provided by the RTEMS network-services path built on `rtems-libbsd`, not by external timing hardware [RD-001][RD-005][RD-006].

Implementation points:

1. RTEMS Net Services is built for RTEMS version 6 [RD-005].

2. Network-stack selection binds to one stack. When libbsd is present, the libbsd stack is selected [RD-005][RD-006].

3. NTP sources are compiled as static library `libntp.a` [RD-005].

4. The imported NTP source set includes FreeBSD `ntpd` sources plus RTEMS adaptation units [RD-007].

5. RTEMS `ntpd` interfaces are provided via `rtems/ntpd.h`, including lifecycle and synchronization status APIs [RD-008].

6. The `ntp01` integration test configures `/etc/ntp.conf`, starts `ntpd`, and validates status with `ntpq` and `ntpsv` [RD-009].

Operational outcome:

- RTEMS telescope subsystems use the same NTP-over-Ethernet discipline model as Linux systems.

- Dependence on legacy IRIG-B over coax is removed from this architecture.

### 5.2 Timing Reference Model (GPS->UTC->TAI)
The consolidated timing model for this change is:
1. GPS is the observatory timing reference.
2. NTP distributes UTC-disciplined time to Linux and RTEMS hosts.
3. EPICS `generalTime` provides UTC to timelib from the active provider.
4. timelib normalizes UTC to TAI and publishes all higher-level scales from that TAI baseline.

This preserves a single chain across all core systems even though operating systems and runtime stacks differ.

### 5.3 TAI Derivation in timelib
- `timeNow` receives UTC seconds since the EPICS epoch from `generalTime`.
- timelib applies `datlsd` (TAI-UTC, in days) to perform UTC->TAI conversion.
- timelib shifts EPICS epoch (1990-01-01) to Unix epoch (1970-01-01) and applies configured test bias where enabled.
- The resulting raw TAI seconds feed `timeRaw2tai` and all higher-level `timeNow*` and `timeThen*` conversions.

### 5.4 Invariants and Responsibilities
1. Single reference: GPS remains authoritative for upstream time distribution.
2. Provider abstraction: applications consume `generalTime`; they do not select NTP/Bancomm directly [RD-002].
3. Normalization contract: timelib converts to TAI before serving derived scales.
4. Configuration discipline: `datlsd` and related constants must remain current under configuration control.

## 6. Detailed Changes Implemented
- Removed Bancomm/IRIG-B-specific sources from timelib build and runtime behavior [RD-003].
- Removed weak-symbol compatibility aliases used to simulate/replace Bancomm functions.
- Updated timing routines and headers to rely on the EPICS generalTime path for supported platforms.
- Added timing-system documentation to capture provider mapping and the GPS->UTC->TAI chain.

## 7. System-Level Impact
- **TCS:** NTP-disciplined clock becomes primary operational time source.
- **MCS:** NTP-disciplined clock becomes primary operational time source.
- **CRCS:** NTP-disciplined clock becomes primary operational time source.
- **SCS:** NTP-disciplined clock becomes primary operational time source.
- **PCS:** NTP-disciplined clock becomes primary operational time source.
- **A&G:** NTP-disciplined clock becomes primary operational time source.

No functional change is intended for UTC->TAI semantics in timelib outputs; the change is provider/protocol standardization from legacy IRIG-B over coax to NTP over Ethernet.

## 8. Operational Requirements
- Each core subsystem host must maintain healthy NTP lock to the observatory timing source.
- Monitoring must alarm on loss of NTP synchronization and unacceptable offset.
- `datlsd` and related timing constants must remain current through normal configuration control.

### 8.1 Leap-Second Update Summary
Leap-second handling is operationally split into two controlled phases:

1. Announcement-to-insertion preparation.

2. Post-insertion normalization.

The official semi-annual leap-second announcements are published by IERS in
Bulletin C [RD-012][RD-013], with historical bulletins available in the full
archive [RD-014].

At a system level, observatory operations should:

- Update pre-insertion parameters for the announced effective date where applicable.

- Apply post-insertion `TAI-UTC` step updates consistently across affected components.

- Perform updates in a coordinated window so all core systems converge on the same UTC->TAI state.

- Verify resulting offsets and timing health after rollout.

Detailed component-level procedures and legacy examples are maintained in `docs/Leap+Seconds.md` [RD-004].

## 9. Risks and Mitigations
- **Risk:** NTP service degradation on a host affects local time quality.
  - **Mitigation:** NTP health monitoring, offset alarms, and operational response procedures.
- **Risk:** Configuration drift in leap-second parameter (`datlsd`) causes UTC/TAI mismatch.
  - **Mitigation:** Controlled update process and periodic verification against known references.
- **Risk:** Residual assumptions of Bancomm presence in external integration scripts.
  - **Mitigation:** Remove/refresh legacy deployment references and validate IOC startup logs.

## 10. Verification and Acceptance
A design-change deployment is accepted when all conditions below are met:
1. timelib builds without Bancomm support module dependencies.
2. IOC startup confirms `generalTime`-based provider usage with no Bancomm symbol resolution.
3. Each core system (TCS, MCS, CRCS, SCS, PCS, A&G) demonstrates stable NTP synchronization.
4. timelib time outputs match expected UTC/TAI behavior within operational tolerance.
5. Existing timing alarms and health indicators operate as expected under nominal and fault scenarios.

## 11. Rollback Strategy
If required, rollback is performed by restoring the pre-`garch-23` timelib release and associated deployment configuration. Rollback should be treated as temporary while root-cause analysis is completed, since the target architecture is NTP-primary without Bancomm dependency.
