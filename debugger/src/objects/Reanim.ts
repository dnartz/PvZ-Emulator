export interface Reanim {
    fps: number,
    prev_fps: number,
    begin_frame: number,
    n_frames: number,
    n_repeated: number,
    progress: number,
    prev_progress: number,
    type: "repeat" | "once"
}