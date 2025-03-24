_base_ = [
    '../_base_/models/upernet_r50.py', 
    '../_base_/default_runtime.py', '../_base_/schedules/schedule_160k.py'
]
crop_size = (800, 600)
data_preprocessor = dict(size=crop_size)
model = dict(
            data_preprocessor=data_preprocessor,
            decode_head=dict(num_classes=3),
            auxiliary_head=dict(num_classes=3),
            pretrained='open-mmlab://resnet101_v1c', backbone=dict(depth=101))
