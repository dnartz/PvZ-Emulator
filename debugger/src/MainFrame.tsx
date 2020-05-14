import * as React from 'React';
import "./assets/style.css";
import { Scene, SceneType } from './objects/Scene';
import { EmulatorState } from './objects/Emulator';
import { ConfigFrame } from './ConfigFrame';
import { SceneFrame } from './SceneFrame';
import { JSONRequest } from './JSONRequest';

export class MainFrame extends React.PureComponent<{}, {
    scene: Scene | null,
    status: EmulatorState
}> {
    updateTimer: number | null;

    constructor(props) {
        super(props);

        this.state = {
            scene: null,
            status: {
                fps: 100,
                isPlaying: false
            }
        };

        this.updateTimer = null;
    }

    componentDidMount() {
        fetch('./status.json')
        .then(res => res.json())
        .then(data => this.setState({
            scene: data
        }));
    }

    doUpdate = () => {
        let next = new Date().getTime() + 1000 / this.state.status.fps;

        return () => {
            JSONRequest("/status.json", null).then(scene => {
                this.setState({
                    scene: scene
                });

                next = next - new Date().getTime();

                if (this.state.status.isPlaying) {
                    this.updateTimer =
                        window.setTimeout(this.doUpdate(), next);
                }
            }).catch(err => {
                console.error(err);
            });
        };
    };

    componentDidUpdate(prevProps, prevState, snapshot) {
        if (!prevState.status.isPlaying && this.state.status.isPlaying) {
            this.updateTimer =
                window.setTimeout(this.doUpdate(), 1000 / this.state.status.fps);
        } else {
            clearTimeout(this.updateTimer);
            this.updateTimer = null;
        }
    }

    onConfigChanged = (e: React.ChangeEvent<HTMLInputElement>) => {
        let config: {
            scene?: SceneType,
            is_zombie_dance?: boolean,
            is_future_enabled?: boolean,
            stop_spawn?: boolean
        } = {};

        if ([
            "day",
            "night",
            "pool",
            "fog",
            "roof",
            "moon_night"
        ].indexOf(e.currentTarget.value) !== -1) {
            config.scene = e.currentTarget.value as SceneType;
        } else if (e.currentTarget.value === "is_future_enabled") {
            config.is_future_enabled = e.currentTarget.checked;
        } else if (e.currentTarget.value === "stop_spawn") {
            config.stop_spawn = e.currentTarget.checked;
        } else {
            config.is_zombie_dance = e.currentTarget.checked;
        }

        JSONRequest('/config.json', config).then(scene => {
            this.setState({
                scene: scene,
            });
        }).catch(err => {
            console.error(err);
        });
    }

    onEmulatorStatusChanged = (e: React.ChangeEvent<HTMLInputElement>) => {
        let status: EmulatorState = {
            ...this.state.status
        };

        if (e.currentTarget.name === "fps") {
            status.fps = Math.max(1, Math.min(1000, parseInt(e.currentTarget.value, 10)));
        }

        this.setState({
            ...this.state,
            status: status
        });
    };

    onPlayBtnClicked = (e: React.MouseEvent<HTMLButtonElement>) => {
        this.setState({
            ...this.state,
            status: {
                ...this.state.status,
                isPlaying: !this.state.status.isPlaying
            }
        });
    };

    render() {
        if (this.state.scene === null) {
            return null;
        }

        return <div id="main-frame">
            <ConfigFrame
                scene={this.state.scene}
                status={this.state.status}
                onEmulatorStatusChanged={this.onEmulatorStatusChanged}
                onConfigChanged={this.onConfigChanged} />
            <SceneFrame
                scene={this.state.scene}
                status={this.state.status}
                onPlayBtnClicked={this.onPlayBtnClicked} />
            <div className="right-frame"></div>
        </div>
    }
}