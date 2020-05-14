import * as React from 'react';
import classnames from 'classnames';
import { Scene } from './objects/Scene';
import { EmulatorState } from './objects/Emulator';

export class ConfigFrame extends React.PureComponent<{
    scene: Scene | null,
    status: EmulatorState,
    onConfigChanged: (e: React.ChangeEvent<HTMLInputElement>) => void,
    onEmulatorStatusChanged: (e: React.ChangeEvent<HTMLInputElement>) => void
}, {
    view: "config" | "plant" | "zombie"
}> {
    constructor(props) {
        super(props);
        this.state = {
            view: "config"
        }
    }

    renderConfig() {
        let scenes = ["Day", "Night", "Pool", "Fog", "Roof", "Moon Night"].map(name => {
            let value = name === "Moon Night" ? "moon_night" : name.toLowerCase();

            return <React.Fragment key={name}>
                <input className="form-check-input" type="radio"
                    checked={this.props.scene.type === value} value={value}
                    onChange={this.props.onConfigChanged}/>
                <label className="form-check-input">{name}</label>
            </React.Fragment>;
        });

        return <React.Fragment>
            <h6>Scene</h6>
            <div className="form-check form-check-inline">{scenes}</div>

            <h6>Easter eggs</h6>
            <div className="form-check form-check-inline">
                <input className="form-check-input" type="checkbox"
                    checked={this.props.scene.is_future_enabled}
                    value="is_future_enabled"
                    onChange={this.props.onConfigChanged}/>
                <label className="form-check-input">Future</label>

                <input className="form-check-input" type="checkbox"
                    checked={this.props.scene.is_zombie_dance}
                    value="is_zombie_dance"
                    onChange={this.props.onConfigChanged}/>
                <label className="form-check-input">Dance</label>
            </div>

            <h6>Spawn</h6>
            <div className="form-check form-check-inline">
                <input className="form-check-input" type="checkbox"
                    checked={this.props.scene.stop_spawn}
                    value="stop_spawn"
                    onChange={this.props.onConfigChanged}/>
                <label className="form-check-input">Stop Spawn</label>
            </div>
        </React.Fragment>;
    }

    render() {
        let views = ["Config", "Plant", "Zombie"].map(name => {
            let lower = name.toLowerCase();
            let active = this.state.view === lower;

            return <li key={name} className={classnames("breadcrumb-item", {
                active: active
            })}>{active ? name : <a href="#">{name}</a>}</li>
        })

        return <div className="col-md-3 left-frame">
            <nav aria-label="breadcrumb">
              <ol className="breadcrumb">{views}</ol>
            </nav>

            {this.state.view == "config" ?
                this.renderConfig() :
                this.state.view == "plant" ?
                    null :
                    null}

            <h6>Frame Rate</h6>
            <div className="input-group">
                <div className="input-group-prepend">
                    <span className="input-group-text" id="">FPS</span>
                </div>
                <input type="number"
                    min={1}
                    max={1000}
                    name="fps"
                    value={this.props.status.fps}
                    onChange={this.props.onEmulatorStatusChanged}
                    className="form-control" />
            </div>
        </div>
    }

}