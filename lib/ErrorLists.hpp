#pragma once

enum error
{
    none, // success
    auth_failed,
    discrepancy,
    conflict,
    timeout,
    checksum,
    not_ready,
    total,
};