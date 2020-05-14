export type GriditemType =
    "grave" |
    "crater" |
    "ladder";

export interface Griditem {
    id: number,
    type: GriditemType,
    row: 0 | 1 | 2 | 3 | 4 | 5,
    col: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8,
    is_disappeared: boolean
}