CameraC = class()

function CameraC:new() end

function CameraC:Init()
  self.camera = Camera(mat4():CreateProjection(70 * 3 / 180, 16 / 9, 0.1, 1000),
                      Transform(vec3(0, 0, 0), vec3(1, 1, 1), quat(0, 0, 0, 1)))
end

function CameraC:Update()
  self.camera:SetTransform(self.actor:GetTransform())
end

function CameraC:PreDraw(renderer, camera)
  renderer:SetCamera(self.camera)
end
