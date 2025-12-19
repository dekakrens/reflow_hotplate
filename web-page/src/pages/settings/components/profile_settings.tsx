import { type FC } from "react";
import type { ReflowProfile } from "../../../types/common";
import { camelCaseToTitleCase } from "../../../utils/camel_to_title_case";
import { MdEdit } from "react-icons/md";

interface ProfileSettingsProps {
  profiles?: ReflowProfile[];
}

const ProfileSettings: FC<ProfileSettingsProps> = ({ profiles }) => {
  return (
    <div className="card card-border bg-base-300 p-3">
      <p>Profiles</p>

      <div className="mt-3 flex flex-col gap-2">
        {profiles &&
          profiles.map((profile, key) => (
            <div
              key={key}
              className="collapse-arrow bg-base-100 border-base-300 collapse border"
            >
              <input type="radio" name="profile list" />
              <div className="collapse-title font-semibold">
                <span>{profile.name}</span>
              </div>

              <div className="collapse-content flex flex-col gap-1 text-sm">
                {Object.entries(profile).map(([key, value], i) => {
                  if (key != "name")
                    return (
                      <div key={i} className="flex items-center gap-2">
                        <div className="flex w-32 justify-between">
                          <p className="col-span-1">
                            {camelCaseToTitleCase(key)}
                          </p>
                          <p className="col-span-1">:</p>
                        </div>

                        <p className="flex-auto">{value}</p>
                      </div>
                    );
                })}

                <button className="btn btn-soft btn-warning mt-2">
                  <MdEdit /> Edit
                </button>
              </div>
            </div>
          ))}
      </div>
    </div>
  );
};

export default ProfileSettings;
